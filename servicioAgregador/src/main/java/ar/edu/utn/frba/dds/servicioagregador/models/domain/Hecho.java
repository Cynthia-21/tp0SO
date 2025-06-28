package ar.edu.utn.frba.dds.servicioagregador.models.domain;

import ar.edu.utn.frba.dds.servicioagregador.models.domain.contenidoMultimedia.ContenidoMultimedia;
import ar.edu.utn.frba.dds.servicioagregador.models.domain.fuentes.Archivo;
import ar.edu.utn.frba.dds.servicioagregador.models.domain.origen.TipoFuente;
import ar.edu.utn.frba.dds.servicioagregador.models.domain.origen.Origen;
import ar.edu.utn.frba.dds.servicioagregador.models.domain.ubicacion.Ubicacion;
import lombok.AllArgsConstructor;
import lombok.Builder;
import lombok.Getter;
import lombok.NoArgsConstructor;
import lombok.Setter;

import java.time.LocalDateTime;
import java.util.ArrayList;
import java.util.List;


@Getter
@Setter
@Builder
@NoArgsConstructor
@AllArgsConstructor
public class Hecho {
  private Long id;
  private String titulo;
  private String descripcion;
  private List<Categoria> categorias;
  private ContenidoMultimedia contenidoMultimedia;
  private Ubicacion ubicacion;
  private LocalDateTime fechaAcontecimiento;
  private LocalDateTime fechaDeCarga;
  private Origen origen;
  private Boolean estaEliminado;
  private Contribuyente contribuyente;
  private List<Etiqueta> etiquetas;
  private Archivo archivoOrigen;
  private TipoFuente tipoFuente;

  public Hecho(String titulo) {
    this.titulo = titulo;
    this.categorias = new ArrayList<>();
    this.fechaDeCarga = LocalDateTime.now();
    this.estaEliminado = false;
    this.etiquetas = new ArrayList<>();
  }


  public boolean contieneTexto(String texto) {
    return titulo.toLowerCase().contains(texto.toLowerCase())
        || descripcion.toLowerCase().contains(texto.toLowerCase());
  }


  public void agregarCategoria(Categoria categoria) {
    if (!this.categorias.contains(categoria)) {
      this.categorias.add(categoria);
    }
  }

  public void agregarEtiqueta(Etiqueta etiqueta) {
    if (!this.etiquetas.contains(etiqueta)) {
      this.etiquetas.add(etiqueta);
    }
  }


  //Para un print
  @Override
  public String toString() {
    return "Hecho { \n\t"
        + "titulo = '" + titulo + "\n\t"
        + "categorias = " + categorias.stream().map(Categoria::getNombre).toList() + "\n\t"
        + "ubicacion = " + ubicacion + "\n\t"
        + "fecha = " + fechaAcontecimiento + "\n\t"
        + "origen = " + origen + "\n\t"
        + "etiquetas = " + etiquetas + "\n}"
        + "}\n";
  }


}
