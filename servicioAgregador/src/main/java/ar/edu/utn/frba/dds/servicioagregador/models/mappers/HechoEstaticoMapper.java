package ar.edu.utn.frba.dds.servicioagregador.models.mappers;

import ar.edu.utn.frba.dds.servicioagregador.models.domain.Hecho;
import ar.edu.utn.frba.dds.servicioagregador.models.domain.fuentes.Archivo;
import ar.edu.utn.frba.dds.servicioagregador.models.domain.origen.Origen;
import ar.edu.utn.frba.dds.servicioagregador.models.domain.origen.TipoFuente;
import ar.edu.utn.frba.dds.servicioagregador.models.domain.ubicacion.Coordenadas;
import ar.edu.utn.frba.dds.servicioagregador.models.domain.ubicacion.Ubicacion;
import ar.edu.utn.frba.dds.servicioagregador.models.dtos.input.HechoInputDTO;
import ar.edu.utn.frba.dds.servicioagregador.models.repositories.ICategoriaRepository;
import ar.edu.utn.frba.dds.servicioagregador.models.repositories.IEtiquetaRepository;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Component;

import java.util.stream.Collectors;

@Component
public class HechoEstaticoMapper implements HechoMapper {
  @Autowired
  private ICategoriaRepository categoriaRepository;
  @Autowired
  private IEtiquetaRepository etiquetaRepository;

  @Override
  public Hecho mapearHecho(HechoInputDTO hechoDto) {
    return Hecho.builder()
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
        .tipoFuente(TipoFuente.ESTATICA)
        .etiquetas(hechoDto.getEtiquetasId()
            .stream()
            .map(this.etiquetaRepository::findById)
            .collect(Collectors.toList()))
        .archivoOrigen(Archivo.builder().nombre(hechoDto.getArchivoOrigen()).build())
        .build();
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
