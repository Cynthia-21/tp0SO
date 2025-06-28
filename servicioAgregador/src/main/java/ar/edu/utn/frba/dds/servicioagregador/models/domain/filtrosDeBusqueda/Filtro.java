package ar.edu.utn.frba.dds.servicioagregador.models.domain.filtrosDeBusqueda;

import ar.edu.utn.frba.dds.servicioagregador.models.domain.Categoria;
import ar.edu.utn.frba.dds.servicioagregador.models.domain.Etiqueta;
import ar.edu.utn.frba.dds.servicioagregador.models.domain.Hecho;
import ar.edu.utn.frba.dds.servicioagregador.models.domain.origen.Origen;
import ar.edu.utn.frba.dds.servicioagregador.models.domain.ubicacion.Ubicacion;
import lombok.Setter;

import java.time.LocalDate;
import java.util.ArrayList;
import java.util.List;
import java.util.function.Predicate;
import java.util.stream.Stream;

@Setter
public class Filtro {
  private String titulo;
  private String descripcion;
  private List<Categoria> categorias;
  private Ubicacion ubicacion;
  private LocalDate fechaAcontecimiento;
  private LocalDate fechaDeCarga;
  private Origen origen;
  private List<Etiqueta> etiquetas;

  public Filtro() {
    this.titulo = null;
    this.descripcion = null;
    this.categorias = new ArrayList<>();
    this.ubicacion = null;
    this.fechaAcontecimiento = null;
    this.fechaDeCarga = null;
    this.origen = null;
    this.etiquetas = new ArrayList<>();
  }

  public boolean cumpleFiltro(Hecho hecho) {
    return filtros().allMatch(p -> p.test(hecho));
  }

  private Stream<Predicate<Hecho>> filtros() {
    return Stream.of(
        h -> titulo == null || titulo.equals(h.getTitulo()),
        h -> descripcion == null || descripcion.equals(h.getDescripcion()),
        h -> categorias.isEmpty() || h.getCategorias().containsAll(categorias),
        h -> ubicacion == null || ubicacion.esIgualA(h.getUbicacion()),
        h -> fechaAcontecimiento == null || fechaAcontecimiento.equals(h.getFechaAcontecimiento()),
        h -> fechaDeCarga == null || fechaDeCarga.equals(h.getFechaDeCarga()),
        h -> origen == null || origen.equals(h.getOrigen()),
        h -> etiquetas.isEmpty() || h.getEtiquetas().containsAll(etiquetas)
    );
  }

  public void agregarCategoria(Categoria categoria) {
    this.categorias.add(categoria);
  }

  public void agregarEtiqueta(Etiqueta etiqueta) {
    this.etiquetas.add(etiqueta);
  }

}
