package ar.edu.utn.frba.dds.servicioagregador.services.hechoBuilder.builders;

import ar.edu.utn.frba.dds.servicioagregador.models.domain.Hecho;
import ar.edu.utn.frba.dds.servicioagregador.models.domain.fuentes.Archivo;
import ar.edu.utn.frba.dds.servicioagregador.models.domain.fuentes.Fuente;
import ar.edu.utn.frba.dds.servicioagregador.models.domain.fuentes.FuenteEstatica;
import ar.edu.utn.frba.dds.servicioagregador.models.domain.origen.Origen;
import ar.edu.utn.frba.dds.servicioagregador.models.domain.ubicacion.Coordenadas;
import ar.edu.utn.frba.dds.servicioagregador.models.domain.ubicacion.Ubicacion;
import ar.edu.utn.frba.dds.servicioagregador.models.dtos.input.HechoInputDTO;
import ar.edu.utn.frba.dds.servicioagregador.models.repositories.ICategoriaRepository;
import ar.edu.utn.frba.dds.servicioagregador.models.repositories.IEtiquetaRepository;
import ar.edu.utn.frba.dds.servicioagregador.services.hechoBuilder.HechoBuilderStrategy;
import org.springframework.stereotype.Component;

import java.util.List;
import java.util.stream.Collectors;

@Component
public class HechoEstaticoBuilder implements HechoBuilderStrategy {
  private final ICategoriaRepository categoriaRepository;
  private final IEtiquetaRepository etiquetaRepository;

  public HechoEstaticoBuilder(ICategoriaRepository categoriaRepository, IEtiquetaRepository etiquetaRepository) {
    this.categoriaRepository = categoriaRepository;
    this.etiquetaRepository = etiquetaRepository;
  }


  @Override
  public List<Hecho> construirHechos(List<HechoInputDTO> hechosInputDto) {
    return hechosInputDto
        .stream()
        .map(hechoDto -> Hecho.builder()
          .titulo(hechoDto.getTitulo())
          .descripcion(hechoDto.getDescripcion())
          .categorias(hechoDto.getCategoriasId()
              .stream()
              .map(this.categoriaRepository::findById)
              .collect(Collectors.toList()))
          .ubicacion(new Ubicacion(new Coordenadas(hechoDto.getLatitud(), hechoDto.getLongitud())))
          .fechaAcontecimiento(hechoDto.getFechaAcontecimiento())
          .fechaDeCarga(hechoDto.getFechaDeCarga())
          .origen(this.matchearOrigen(hechoDto.getOrigen()))
          .etiquetas(hechoDto.getEtiquetasId()
              .stream()
              .map(this.etiquetaRepository::findById)
              .collect(Collectors.toList()))
          .archivoOrigen(Archivo.builder().nombre(hechoDto.getArchivoOrigen()).build())
          .build())
        .collect(Collectors.toList());
  }

  @Override
  public boolean soporta(Fuente fuente) {
    return fuente instanceof FuenteEstatica;
  }

  private Origen matchearOrigen(String origen) {
    return switch (origen) {
      case "MANUAL" -> Origen.MANUAL;
      case "DATASET" -> Origen.DATASET;
      case "CONTRIBUYENTE" -> Origen.CONTRIBUYENTE;
      default -> null;
    };
  }
}
